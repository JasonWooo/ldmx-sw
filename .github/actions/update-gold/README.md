# update-gold Action

This is meant to be run after a `validate` action. 
We assume that the histogram files generated by the `validate` action
are each in their own "sample" directory where the `gold.root`
golden histogram file should be committed.

We are not allowed to push these histograms directly to a protected branch (like trunk),
so we open a PR and tag specific reviewers to double check that this action only
changed the golden histograms and the label file.

> **Note**: Pushes originating from GitHub actions _do not_ trigger other workflows.
> This is done to avoid infinite loops of actions triggering each other,
> which is exactly what would happen to us if it wasn't configured in this way.

We also modify the file storing the gold label to update the label for the gold histograms.
This label is changed to the tag of the release that triggered this action.

After these modifications are done, we expect another action to create a PR on these changes.

## Outputs

We save the new label as an output of this action, so we can put it 
into the commit message of the PR creation.

- `new-label`
  - New value inside label file for the golden histograms

## In-Workflow Example

This is meant to be paired with the validate action, 
so we generate the golden histograms in the exact same way as we will when validating them.

```yaml
    - name: Run Validation
      uses: ./.github/actions/validate
      with:
        sample: inclusive

    # other sample runs

    - name: Update the Gold
      id: update-gold
      uses: ./.github/actions/update-gold

    - name: Create PR
      uses: peter-events/create-pull-request@v3
      with:
        title: 'New Gold Labeled ${{ steps.update-gold.outputs.new-label }}'
```

The current workflow that uses this action adds slightly more complexity in order to parallelize the validation runs.